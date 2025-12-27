import { Test, TestingModule } from '@nestjs/testing';
import { FirmwareController } from './firmware.controller';
import { FirmwareService } from './firmware.service';

describe('FirmwareController', () => {
  let controller: FirmwareController;

  beforeEach(async () => {
    const module: TestingModule = await Test.createTestingModule({
      controllers: [FirmwareController],
      providers: [FirmwareService],
    }).compile();

    controller = module.get<FirmwareController>(FirmwareController);
  });

  it('should be defined', () => {
    expect(controller).toBeDefined();
  });
});
